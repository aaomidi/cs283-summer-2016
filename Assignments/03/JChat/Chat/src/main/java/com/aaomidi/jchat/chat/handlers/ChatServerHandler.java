package com.aaomidi.jchat.chat.handlers;

import com.aaomidi.jchat.chat.Main;
import com.aaomidi.jchat.chat.data.Message;
import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.util.CharsetUtil;

public class ChatServerHandler extends ChannelInboundHandlerAdapter {
    private final Main instance;

    public ChatServerHandler(Main instance) {
        this.instance = instance;
    }

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        ByteBuf buf = ctx.alloc().buffer();
        // Initial message.
        buf.writeCharSequence(
                String.format("EHLO:3\r\nHello and welcome to chatting with Amir!.\r\nMy public key is:\r\n%d\r\n%d\r\nMessages from this point on will be encrypted.\r\n", instance.getRsa().getPubKey()[0], instance.getRsa().getPubKey()[1]),
                CharsetUtil.UTF_8);
        ctx.channel().writeAndFlush(buf).await();
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        ByteBuf message = (ByteBuf) msg;
        try {
            Message m = instance.getGson().fromJson(message.toString(CharsetUtil.UTF_8), Message.class);
            System.out.println(m.decrypt(instance.getRsa()));
        } catch (Exception ex) {
            ex.printStackTrace();
        } finally {
            message.release();
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        System.err.printf("An exception was caught!\n");
        cause.printStackTrace();
        ctx.close();
    }


}
